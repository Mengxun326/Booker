#ifndef DEF_METADATA
#define DEF_METADATA

#include <ostream>
#include <string>
#include <map>

namespace booker
{
	class Metadata
	{
		public:
			inline bool has(std::string const& key) const { return m_data.find(key) != m_data.end(); }
			inline std::string get(std::string const& key) const { return m_data.at(key); }
			inline void set(std::string const& key, std::string const& value) { m_data[key] = value; }
			
			inline bool hasTitle() const { return has("title"); }
			inline std::string title() const { return has("title") ? get("title") : ""; }
			inline void setTitle(std::string const& title) { set("title", title); }
			
			inline bool hasCreator() const { return has("creator"); }
			inline std::string creator() const { return has("creator") ? get("creator") : ""; }
			inline void setCreator(std::string const& creator) { set("creator", creator); }
			
			inline bool hasSubject() const { return has("subject"); }
			inline std::string subject() const { return has("subject") ? get("subject") : ""; }
			inline void setSubject(std::string const& subject) { set("subject", subject); }
			
			inline bool hasDescription() const { return has("description"); }
			inline std::string description() const { return has("description") ? get("description") : ""; }
			inline void setDescription(std::string const& description) { set("description", description); }
			
			inline bool hasPublisher() const { return has("publisher"); }
			inline std::string publisher() const { return has("publisher") ? get("publisher") : ""; }
			inline void setPublisher(std::string const& publisher) { set("publisher", publisher); }
			
			inline bool hasContributor() const { return has("contributor"); }
			inline std::string contributor() const { return has("contributor") ? get("contributor") : ""; }
			inline void setContributor(std::string const& contributor) { set("contributor", contributor); }
			
			inline bool hasDate() const { return has("date"); }
			inline std::string date() const { return has("date") ? get("date") : ""; }
			inline void setDate(std::string const& date) { set("date", date); }
			
			inline bool hasType() const { return has("type"); }
			inline std::string type() const { return has("type") ? get("type") : ""; }
			inline void setType(std::string const& type) { set("type", type); }
			
			inline bool hasFormat() const { return has("format"); }
			inline std::string format() const { return has("format") ? get("format") : ""; }
			inline void setFormat(std::string const& format) { set("format", format); }
			
			inline bool hasIdentifier() const { return has("identifier"); }
			inline std::string identifier() const { return has("identifier") ? get("identifier") : ""; }
			inline void setIdentifier(std::string const& identifier) { set("identifier", identifier); }
			
			inline bool hasSource() const { return has("source"); }
			inline std::string source() const { return has("source") ? get("source") : ""; }
			inline void setSource(std::string const& source) { set("source", source); }
			
			inline bool hasLanguage() const { return has("language"); }
			inline std::string language() const { return has("language") ? get("language") : ""; }
			inline void setLanguage(std::string const& language) { set("language", language); }
			
			inline bool hasRelation() const { return has("relation"); }
			inline std::string relation() const { return has("relation") ? get("relation") : ""; }
			inline void setRelation(std::string const& relation) { set("relation", relation); }
			
			inline bool hasCoverage() const { return has("coverage"); }
			inline std::string coverage() const { return has("coverage") ? get("coverage") : ""; }
			inline void setCoverage(std::string const& coverage) { set("coverage", coverage); }
			
			inline bool hasRights() const { return has("rights"); }
			inline std::string rights() const { return has("rights") ? get("rights") : ""; }
			inline void setRights(std::string const& rights) { set("rights", rights); }
			
			friend inline std::ostream& operator<<(std::ostream& os, Metadata const& src) { src.write(os); return os; }
		
		private:
			std::map<std::string, std::string> m_data;
		
		protected:
			virtual void write(std::ostream& os) const;
	};
}

#endif // DEF_METADATA